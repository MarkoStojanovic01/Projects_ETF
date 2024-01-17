import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ManagerExaminationsComponent } from './manager-examinations.component';

describe('ManagerExaminationsComponent', () => {
  let component: ManagerExaminationsComponent;
  let fixture: ComponentFixture<ManagerExaminationsComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ ManagerExaminationsComponent ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(ManagerExaminationsComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
