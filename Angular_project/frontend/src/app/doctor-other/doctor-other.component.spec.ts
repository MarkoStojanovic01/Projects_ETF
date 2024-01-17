import { ComponentFixture, TestBed } from '@angular/core/testing';

import { DoctorOtherComponent } from './doctor-other.component';

describe('DoctorOtherComponent', () => {
  let component: DoctorOtherComponent;
  let fixture: ComponentFixture<DoctorOtherComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ DoctorOtherComponent ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(DoctorOtherComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
