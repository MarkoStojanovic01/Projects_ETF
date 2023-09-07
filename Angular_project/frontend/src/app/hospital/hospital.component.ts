import { Component, OnInit } from '@angular/core';
import { UserService } from '../services/user.service';
import { User } from '../models/user';

@Component({
  selector: 'app-hospital',
  templateUrl: './hospital.component.html',
  styleUrls: ['./hospital.component.css']
})
export class HospitalComponent implements OnInit {

  constructor(private userService: UserService) { }

  allDoctors: User[] = []
  shownDoctors: User[] = []

  ngOnInit(): void {
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
    specialization: ''
  };

  get filteredDoctors(): User[] {
    return this.allDoctors.filter(doc =>
      (doc.firstname.toLowerCase().includes(this.searchText.firstname.toLowerCase()) &&
      doc.lastname.toLowerCase().includes(this.searchText.lastname.toLowerCase()) &&
      doc.specialization.toLowerCase().includes(this.searchText.specialization.toLowerCase()))
    );
  }

  applyFilters(): void {
    this.shownDoctors = this.filteredDoctors
  }


}
