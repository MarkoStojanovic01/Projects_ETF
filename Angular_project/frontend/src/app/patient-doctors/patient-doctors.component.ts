import { Component, OnInit } from '@angular/core';
import { User } from '../models/user';
import { UserService } from '../services/user.service';
import { Router } from '@angular/router';

@Component({
  selector: 'app-patient-doctors',
  templateUrl: './patient-doctors.component.html',
  styleUrls: ['./patient-doctors.component.css']
})
export class PatientDoctorsComponent implements OnInit {

  constructor(private userService: UserService, private router: Router) { }

  logout() {
    localStorage.clear()
    this.router.navigate([''])
  }

  allDoctors: User[] = []
  shownDoctors: User[] = []

  ngOnInit(): void {
    let type = localStorage.getItem("type");
    if(type != 'patient') this.router.navigate(['']);
    this.userService.getAllDoctors().subscribe((data:User[])=>{
      this.allDoctors = data;
      this.shownDoctors = data;
    })
  }

  // Sorting properties
  private sortDirection = 1; // 1 for ascending, -1 for descending
  private currentSortKey = '';

  sortBy(key: string): void {
    this.sortDirection = 1;
    this.currentSortKey = key;
    this.shownDoctors.sort((a, b) =>
      a[key].localeCompare(b[key]) * this.sortDirection
    );
  }

  reverseSortBy(key: string): void {
    this.sortDirection = -1;
    this.currentSortKey = key;
    this.shownDoctors.sort((a, b) =>
      a[key].localeCompare(b[key]) * this.sortDirection
    );
  }

  // Search text object
  searchText = {
    firstname: '',
    lastname: '',
    specialization: '',
    branch: ''
  };

  get filteredDoctors(): User[] {
    return this.allDoctors.filter(doc =>
      (doc.firstname.toLowerCase().includes(this.searchText.firstname.toLowerCase()) &&
      doc.lastname.toLowerCase().includes(this.searchText.lastname.toLowerCase()) &&
      doc.specialization.toLowerCase().includes(this.searchText.specialization.toLowerCase()) &&
      doc.branch.toLowerCase().includes(this.searchText.branch.toLowerCase()))
    );
  }

  applyFilters(): void {
    this.shownDoctors = this.filteredDoctors
  }

}
